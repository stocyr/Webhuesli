#ifndef PWM_H_
#define PWM_H_
/******************************************************************************/
/** \file       Pwm.h
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
 ******************************************************************************/
/*
 *  function    getPwmPeriod
 *              setPwmPeriod
 *              getPwmDuty
 *              setPwmDuty
 *              getPwmState
 *              setPwmState
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <stdint.h>

#include "BBBTypes.h"

//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------

/** Defines the state of a pwm device */
typedef enum _ePwmState {

    PWM_STOP = 0,  ///< pwm device stopped
    PWM_RUN  = 1   ///< pwm device is running

} ePwmState;


/** All attached pwm devices */
typedef enum _ePwmDevice {

    PWM_P9_14 = 0,  ///< pwm device on expansion header P9.14
    PWM_P9_22 = 1,  ///< pwm device on expansion header P9.22
    PWM_P8_19 = 2   ///< pwm device on expansion header P8.19

} ePwmDevice;

//----- Function prototypes ----------------------------------------------------
extern BBBError getPwmPeriod(ePwmDevice eDevice, uint32_t *u32Period);

extern BBBError setPwmPeriod(ePwmDevice eDevice, uint32_t u32Period);

extern BBBError getPwmDuty(ePwmDevice eDevice, uint32_t *u32Duty);

extern BBBError setPwmDuty(ePwmDevice eDevice, uint32_t u32Duty);

extern BBBError getPwmState(ePwmDevice eDevice, ePwmState *eState);

extern BBBError setPwmState(ePwmDevice eDevice, ePwmState eState);

//----- Data -------------------------------------------------------------------


#endif /* PWM_H_ */
