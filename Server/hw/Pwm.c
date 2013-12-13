/******************************************************************************/
/** \file       Pwm.c
 *******************************************************************************
 *
 *  \brief      Module to control the attached pwm of the beaglebone black
 *              webhouse.
 *              <p>
 *              Before the pwm can be accessed in this way, the pwm's must
 *              be initialized. This is done at startup with the help of a
 *              systemd service unit (initPwm.service). The service calls the
 *              shell script /opt/webhuesli/init/initPwm.sh). Three pwm's are
 *              initialized for the use within the webhouse:
 *              <pre>
 *              Usage      Header pin    pwm access folder
 *              dlampe     P9.14         /sys/devices/ocp.2/pwm_test_P9_14.14
 *              slampe     P9.22         /sys/devices/ocp.2/pwm_test_P9_22.15
 *              heiz       P8.19         /sys/devices/ocp.2/pwm_test_P8_19.16
 *              </pre>
 *              <p>
 *              Every pwm can be turned on/off, the duty cycle can be set and
 *              the period of the pwm.
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
 *              getPwmPeriod
 *              setPwmPeriod
 *              getPwmDuty
 *              setPwmDuty
 *              getPwmState
 *              setPwmState
 *  functions  local:
 *              composeFilename
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "Pwm.h"
#include "Log.h"

//----- Macros -----------------------------------------------------------------
#define PWM_MAX_BUF          ( 64 )

//----- Data types -------------------------------------------------------------
/** Location of the attached pwm's */
static char * pcPwmDevice[] = {

    "/sys/devices/ocp.2/pwm_test_P9_14.14", ///< pwm on extension header P9.14
    "/sys/devices/ocp.2/pwm_test_P9_22.15", ///< pwm on extension header P9.22
    "/sys/devices/ocp.2/pwm_test_P8_19.16"  ///< pwm on extension header P8.19
};

static char * pcPwmDuty   = "/duty";
static char * pcPwmPeriod = "/period";
static char * pcPwmState  = "/run";

//----- Function prototypes ----------------------------------------------------
static void composeFilename(char * pcBuffer,
                            ePwmDevice eDevice,
                            char * pcFilename);

//----- Data -------------------------------------------------------------------

//----- Implementation ---------------------------------------------------------


/*******************************************************************************
 *  function :    getPwmPeriod
 ******************************************************************************/
/** \brief        Get the period [ns] of the corresponding pwm
 *
 *  \type         global
 *
 *  \param[in]    eDevice     pwm device
 *  \param[out]   u32Period   period [ns] of the pwm
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                </pre>
 *
 ******************************************************************************/
BBBError getPwmPeriod(ePwmDevice eDevice, uint32_t *u32Period) {

    int      fd = -1;
    char     cBuf[PWM_MAX_BUF];
    BBBError error = BBB_SUCCESS;

    composeFilename(cBuf, eDevice, pcPwmPeriod);

    fd = open(cBuf, O_RDONLY);
    if (fd < 0) {
        ERRORPRINT("get Period of device %d failed", eDevice);
        error = BBB_FILE_OPEN;
    } else {

        read(fd, cBuf, PWM_MAX_BUF);
        *u32Period = strtoul (cBuf, NULL, 10);
        close(fd);
    }

    return(error);
}

/*******************************************************************************
 *  function :    setPwmPeriod
 ******************************************************************************/
/** \brief        Set the period [ns] of the corresponding pwm
 *
 *  \type         global
 *
 *  \param[in]    eDevice     pwm device
 *  \param[in]    u32Period   period [ns] of the pwm
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                </pre>
 *
 ******************************************************************************/
BBBError setPwmPeriod(ePwmDevice eDevice, uint32_t u32Period) {

    int      fd = -1;
    int      len = 0;
    char     cBuf[PWM_MAX_BUF];
    BBBError error = BBB_SUCCESS;

    composeFilename(cBuf, eDevice, pcPwmPeriod);

    fd = open(cBuf, O_WRONLY);
    if (fd < 0) {
        ERRORPRINT("set Period of device %d failed", eDevice);
        error = BBB_FILE_OPEN;
    } else {

        len = snprintf(cBuf, sizeof(cBuf), "%d", u32Period);
        write(fd, cBuf, len);
        close(fd);
    }

    return(error);
}

/*******************************************************************************
 *  function :    getPwmDuty
 ******************************************************************************/
/** \brief        Get the duty [ns] of the corresponding pwm
 *
 *  \type         global
 *
 *  \param[in]    eDevice     pwm device
 *  \param[out]   u32Duty     duty [ns] of the pwm
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                </pre>
 *
 ******************************************************************************/
BBBError getPwmDuty(ePwmDevice eDevice, uint32_t *u32Duty) {

    int      fd = -1;
    char     cBuf[PWM_MAX_BUF];
    BBBError error = BBB_SUCCESS;

    composeFilename(cBuf, eDevice, pcPwmDuty);

    fd = open(cBuf, O_RDONLY);
    if (fd < 0) {
        ERRORPRINT("get Duty of device %d failed", eDevice);
        error = BBB_FILE_OPEN;
    } else {

        read(fd, cBuf, PWM_MAX_BUF);
        *u32Duty = strtoul (cBuf, NULL, 10);
        close(fd);
    }

    return(error);
}

/*******************************************************************************
 *  function :    setPwmDuty
 ******************************************************************************/
/** \brief        Set the duty [ns] of the corresponding pwm
 *
 *  \type         global
 *
 *  \param[in]    eDevice     pwm device
 *  \param[in]    u32Duty     duty [ns] of the pwm
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                </pre>
 *
 ******************************************************************************/
BBBError setPwmDuty(ePwmDevice eDevice, uint32_t u32Duty) {

    int      fd = -1;
    int      len = 0;
    char     cBuf[PWM_MAX_BUF];
    BBBError error = BBB_SUCCESS;

    composeFilename(cBuf, eDevice, pcPwmDuty);

    fd = open(cBuf, O_WRONLY);
    if (fd < 0) {
        ERRORPRINT("set Duty of device %d failed", eDevice);
        error = BBB_FILE_OPEN;
    } else {

        len = snprintf(cBuf, sizeof(cBuf), "%d", u32Duty);
        write(fd, cBuf, len);
        close(fd);
    }

    return(error);
}

/*******************************************************************************
 *  function :    getPwmState
 ******************************************************************************/
/** \brief        Get the state (PWM_STOP or PWM_RUN) of the corresponding pwm
 *
 *  \type         global
 *
 *  \param[in]    eDevice     pwm device
 *  \param[out]   eState      PWM_STOP or PWM_RUN
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                BBB_ERR_UNKNOWN  pwm in an unknown state
 *                </pre>
 *
 ******************************************************************************/
BBBError getPwmState(ePwmDevice eDevice, ePwmState *eState) {

    int      fd = -1;
    char     cBuf[PWM_MAX_BUF];
    char     ch;
    BBBError error = BBB_SUCCESS;

    composeFilename(cBuf, eDevice, pcPwmState);

    fd = open(cBuf, O_RDONLY);
    if (fd < 0) {
        ERRORPRINT("get State of device %d failed", eDevice);
        error = BBB_FILE_OPEN;
    } else {

        read(fd, &ch, 1);
        if(ch == '1') {

            *eState = PWM_RUN;

        } else if(ch == '0') {

            *eState = PWM_STOP;

        } else {

            error = BBB_ERR_UNKNOWN;
            *eState = PWM_STOP;
        }
        close(fd);
    }

    return(error);
}

/*******************************************************************************
 *  function :    setPwmState
 ******************************************************************************/
/** \brief        Set the state (PWM_STOP or PWM_RUN) of the corresponding pwm
 *
 *  \type         global
 *
 *  \param[in]    eDevice     pwm device
 *  \param[in]    eState      PWM_STOP or PWM_RUN
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                BBB_ERR_UNKNOWN  pwm in an unknown state
 *                </pre>
 *
 ******************************************************************************/
BBBError setPwmState(ePwmDevice eDevice, ePwmState eState) {

    int      fd = -1;
    char     cBuf[PWM_MAX_BUF];
    BBBError error = BBB_SUCCESS;

    composeFilename(cBuf, eDevice, pcPwmState);

    fd = open(cBuf, O_WRONLY);
    if (fd < 0) {
        ERRORPRINT("set State of device %d failed", eDevice);
        error = BBB_FILE_OPEN;
    } else {

        if(eState == PWM_RUN) {

            write(fd, "1", 2);

        } else if(eState == PWM_STOP) {

            write(fd, "0", 2);

        } else {

            error = BBB_ERR_PARAM;
        }
        close(fd);
    }

    return(error);
}

/*******************************************************************************
 *  function :    composeFilename
 ******************************************************************************/
static void composeFilename(char * pcBuffer,
                            ePwmDevice eDevice,
                            char * pcFilename) {

    pcBuffer[0] = '\0';
    strcat(pcBuffer, pcPwmDevice[eDevice]);
    strcat(pcBuffer, pcFilename);
}
