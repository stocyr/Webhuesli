/******************************************************************************/
/** \file       Webhouse.c
 *******************************************************************************
 *
 *  \brief      API to control the hardware of the beaglebone black webhouse.
 *              <p>
 *              Before any other function can be used, the webhouse must be
 *              initialized by calling initWebhouse(). To release all resource,
 *              finalizeWebhouse() can be called.
 *              <p>
 *              The following hw can be controlled by this api:
 *              <ul>
 *              <li> tv: The TV can be turned on/off, and the state can be
 *              polled.
 *              <li> led: The LED (just used for test purposes) can be turned
 *              on/off, and the state can be polled.
 *              <li> sLampe: The floor lamp can be dimmed from 0 (dark) to
 *              100 (brightest) and the dim state can be polled
 *              <li> dlampe: The ceiling lamp can be dimmed from 0 (dark) to
 *              100 (brightest) and the dim state can be polled
 *              <li> heizung: The heater can be dimmed from 0 (no heating) to
 *              100 (full heating) and the dim state can be polled
 *              <li> Current Temperature: The current temperature can be polled
 *              <li> Alarm: The alarm can be enabled/disabled, the alarm state
 *              can be polled, it can be polled if an alarm was triggered by the
 *              pir and the triggered alarm can be reseted.
 *              </ul>
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
 *              initWebhouse
 *              finalizeWebhouse
 *              turnTVOn
 *              turnTVOff
 *              getTVState
 *              turnLEDOn
 *              turnLEDOff
 *              getLEDState
 *              dimSLampe
 *              getSLampeState
 *              dimDLampe
 *              getDLampeState
 *              dimHeizung
 *              getHeizungState
 *              getTempIst
 *              enableAlarm
 *              disableAlarm
 *              getAlarmState
 *              isAlarmSet
 *              resetAlarm
 *  functions  local:
 *              initTV
 *              finalizeTV
 *              initLED
 *              finalizeLED
 *              initSLampe
 *              finalizeSLampe
 *              initDLampe
 *              finalizeDLampe
 *              initHeizung
 *              finalizeHeizung
 *              calcDuration
 *              timespec2nsec
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <time.h>
#include <limits.h>

#include "Webhouse.h"
#include "Gpio.h"
#include "Pwm.h"
#include "Log.h"
#include "Lm75.h"
#include "Pir.h"

//----- Macros -----------------------------------------------------------------
#define GPIO_TV          ( 60 )
#define GPIO_LED         ( 48 )
#define GPIO_TEMP_INT    ( 31 )
#define GPIO_RTC_INT     ( 49 )

#define PWM_PERIOD       ( 10000000 )
#define PWM_PERIOD_PER   ( PWM_PERIOD / 100 )

#define TEMP_OFFSET      ( 10 )

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
static BBBError initTV(void);
static BBBError finalizeTV(void);
static BBBError initLED(void);
static BBBError finalizeLED(void);
static BBBError initSLampe(void);
static BBBError finalizeSLampe(void);
static BBBError initDLampe(void);
static BBBError finalizeDLampe(void);
static BBBError initHeizung(void);
static BBBError finalizeHeizung(void);
static uint64_t calcDuration(uint64_t u64StartTime, uint64_t u64StopTime);
static uint64_t timespec2nsec(struct timespec * time);

//----- Data -------------------------------------------------------------------

//----- Implementation ---------------------------------------------------------

/*******************************************************************************
 *  function :    initWebhouse
 ******************************************************************************/
/** \brief        Initializes all used hardware within the webhouse.
 *                <p>
 *                Before any other function can be used, the webhouse must be
 *                initialized by calling this function.
 *
 *  \type         global
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                BBB_ERR_PARAM    if a parameter error occurred
 *                </pre>
 *
 ******************************************************************************/
BBBError initWebhouse(void) {

    BBBError error = BBB_SUCCESS;

    LOGINIT();

    INFOPRINT("initialize BBB webhouse");
    error = initTV();
    error |= initLED();
    error |= initSLampe();
    error |= initDLampe();
    error |= initHeizung();
    error |= initPir();

    return (error);
}


/*******************************************************************************
 *  function :    finalizeWebhouse
 ******************************************************************************/
/** \brief        Release all resource, and sets the webhouse in a defined
 *                state (everything is turned off).
 *
 *  \type         global
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                BBB_ERR_PARAM    if a parameter error occurred
 *                </pre>
 *
 ******************************************************************************/
BBBError finalizeWebhouse(void) {

    BBBError error = BBB_SUCCESS;

    INFOPRINT("finalize BBB webhouse");
    error = finalizeTV();
    error |= finalizeLED();
    error |= finalizeSLampe();
    error |= finalizeDLampe();
    error |= finalizeHeizung();
    error |= finalizePir();

    return (error);
}

/*******************************************************************************
 *  function :    turnTVOn
 ******************************************************************************/
/** \brief        Turn on the TV
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                BBB_ERR_PARAM    if a parameter error occurred
 *                </pre>
 *
 ******************************************************************************/
BBBError turnTVOn(void) {

    return (setGpioValue(GPIO_TV, GPIO_VALUE_HIGH));
}

/*******************************************************************************
 *  function :    turnTVOff
 ******************************************************************************/
/** \brief        Turn off the TV
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                BBB_ERR_PARAM    if a parameter error occurred
 *                </pre>
 *
 ******************************************************************************/
BBBError turnTVOff(void) {

    return (setGpioValue(GPIO_TV, GPIO_VALUE_LOW));
}

/*******************************************************************************
 *  function :    getTVState
 ******************************************************************************/
/** \brief        Get the state (1 equals to ON, 0 equals to OFF) of the TV
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       1 for TV is ON, 0 for TV is OFF
 *
 ******************************************************************************/
int32_t getTVState(void) {

    eGpioValue eValue = GPIO_VALUE_LOW;

    getGpioValue(GPIO_TV, &eValue);

    return (eValue);
}

/*******************************************************************************
 *  function :    turnLEDOn
 ******************************************************************************/
/** \brief        Turn on the LED (LED is just used for test purposes)
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                BBB_ERR_PARAM    if a parameter error occurred
 *                </pre>
 *
 ******************************************************************************/
BBBError turnLEDOn(void) {

    return (setGpioValue(GPIO_LED, GPIO_VALUE_HIGH));
}

/*******************************************************************************
 *  function :    turnLEDOff
 ******************************************************************************/
/** \brief        Turn off the LED (LED is just used for test purposes)
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                BBB_ERR_PARAM    if a parameter error occurred
 *                </pre>
 *
 ******************************************************************************/
BBBError turnLEDOff(void) {

    return (setGpioValue(GPIO_LED, GPIO_VALUE_LOW));
}

/*******************************************************************************
 *  function :    getLEDState
 ******************************************************************************/
/** \brief        Get the state (1 equals to ON, 0 equals to OFF) of the LED
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       1 for LED is ON, 0 for LED is OFF
 *
 ******************************************************************************/
int32_t getLEDState(void) {

    eGpioValue eValue = GPIO_VALUE_LOW;

    getGpioValue(GPIO_LED, &eValue);

    return (eValue);
}

/*******************************************************************************
 *  function :    dimSLampe
 ******************************************************************************/
/** \brief        Dim the floor lamp from 0 (dark) to 100 (brightest)
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \param[in]    u8Duty   Dim level [0,100]
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                </pre>
 *
 ******************************************************************************/
BBBError dimSLampe(uint8_t u8Duty) {

    uint32_t u32Duty = 0;

    if(u8Duty > 100) {
        u8Duty = 100;
    }

    u32Duty = PWM_PERIOD - (PWM_PERIOD_PER * u8Duty);

    return (setPwmDuty(PWM_P9_22, u32Duty));
}

/*******************************************************************************
 *  function :    getSLampeState
 ******************************************************************************/
/** \brief        Get the state of the floor lamp (0 (dark) to 100 (brightest))
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       Dim level [0,100]
 *
 ******************************************************************************/
int32_t getSLampeState(void) {

    uint32_t u32Duty = 0;
    int32_t  s32DutyPercentage = 0;

    getPwmDuty(PWM_P9_22, &u32Duty);

    s32DutyPercentage = (PWM_PERIOD - u32Duty) / PWM_PERIOD_PER;

    return (s32DutyPercentage);
}

/*******************************************************************************
 *  function :    dimDLampe
 ******************************************************************************/
/** \brief        Dim the ceiling lamp from 0 (dark) to 100 (brightest)
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \param[in]    u8Duty   Dim level [0,100]
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                </pre>
 *
 ******************************************************************************/
BBBError dimDLampe(uint8_t u8Duty) {

    uint32_t u32Duty = 0;

    if(u8Duty > 100) {
        u8Duty = 100;
    }

    u32Duty = PWM_PERIOD - (PWM_PERIOD_PER * u8Duty);

    return (setPwmDuty(PWM_P9_14, u32Duty));
}

/*******************************************************************************
 *  function :    getDLampeState
 ******************************************************************************/
/** \brief        Get the state of the ceiling lamp (0 (dark) to 100 (brightest))
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       Dim level [0,100]
 *
 ******************************************************************************/
int32_t getDLampeState(void) {

    uint32_t u32Duty= 0;
    int32_t  s32DutyPercentage = 0;

    getPwmDuty(PWM_P9_14, &u32Duty);

    s32DutyPercentage = (PWM_PERIOD - u32Duty) / PWM_PERIOD_PER;

    return (s32DutyPercentage);
}

/*******************************************************************************
 *  function :    dimHeizung
 ******************************************************************************/
/** \brief        Dim the heater from 0 (no heating) to 100 (full heating)
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \param[in]    u8Duty   Dim level [0,100]
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                </pre>
 *
 ******************************************************************************/
BBBError dimHeizung(uint8_t u8Duty) {

    uint32_t u32Duty;

    if(u8Duty > 100) {
        u8Duty = 100;
    }

    u32Duty = PWM_PERIOD - (PWM_PERIOD_PER * u8Duty);

    return (setPwmDuty(PWM_P8_19, u32Duty));
}

/*******************************************************************************
 *  function :    getHeizungState
 ******************************************************************************/
/** \brief        Get the state of the heater (0 (no heating) to 100
 *                (full heating))
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       Dim level [0,100]
 *
 ******************************************************************************/
int32_t getHeizungState(void) {

    uint32_t u32Duty;
    int32_t  s32DutyPercentage;

    getPwmDuty(PWM_P8_19, &u32Duty);

    s32DutyPercentage = (PWM_PERIOD - u32Duty) / PWM_PERIOD_PER;

    return (s32DutyPercentage);
}

/*******************************************************************************
 *  function :    getTempIst
 ******************************************************************************/
/** \brief        Get the current temperature within the webhouse in degree
 *                celcius.
 *                <p>
 *                Accessing the temperature sensor continuously without waiting
 *                at least one conversion time between communications will prevent
 *                the device from updating the temperature register with a new
 *                temperature conversion result. Thus we limit the access to the
 *                sensor and manage a shadow temperature register within the
 *                function.
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       current temperature within the webhouse in degree celcius
 *
 ******************************************************************************/
int32_t getTempIst(void) {

    static int32_t s32Temp = 0;
    static struct timespec stamp = {0,0};
    struct timespec know;
    uint64_t u64Diff;

    if((stamp.tv_nsec == 0) && (stamp.tv_sec == 0) && (s32Temp == 0)) {

        clock_gettime(CLOCK_REALTIME, &stamp);
        readTempLm75(&s32Temp);
        s32Temp = s32Temp - TEMP_OFFSET;
    } else {

        clock_gettime(CLOCK_REALTIME, &know);
        u64Diff = calcDuration(timespec2nsec(&stamp), timespec2nsec(&know));

        if(u64Diff >= 1000000000uLL) {

            readTempLm75(&s32Temp);
            s32Temp = s32Temp - TEMP_OFFSET;
            clock_gettime(CLOCK_REALTIME, &stamp);
        }
    }

    return (s32Temp);
}

/*******************************************************************************
 *  function :    enableAlarm
 ******************************************************************************/
/** \brief        Enables the alarm and thus start the polling of the pir sensor.
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       <pre>
 *                BBB_SUCCESS        on success
 *                BBB_THREAD_CREATE  the thread could not be created
 *                BBB_THREAD_RUNNING thread is already running
 *                </pre>
 *
 ******************************************************************************/
BBBError enableAlarm(void) {

    return (startPollPir());
}

/*******************************************************************************
 *  function :    disableAlarm
 ******************************************************************************/
/** \brief        Disable the alarm and thus stops the polling of the pir sensor.
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       <pre>
 *                BBB_SUCCESS        on success
 *                BBB_THREAD_RUNNING thread is not running
 *                </pre>
 *
 ******************************************************************************/
BBBError disableAlarm(void) {

    return (stopPollPir());
}

/*******************************************************************************
 *  function :    getAlarmState
 ******************************************************************************/
/** \brief        Get the state of the alarm
 *                <p>
 *                Returns if the alarm is enabled or disabled. If the alarm is
 *                enabled, the pir sensor is polled on any movement within
 *                the webhouse.
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       1 for alarm is enabled, 0 for alarm is disabled
 *
 ******************************************************************************/
int32_t getAlarmState(void) {

    return (isPollThreadRunning());
}

/*******************************************************************************
 *  function :    isAlarmSet
 ******************************************************************************/
/** \brief        Returns if the pir has detected any movement within the house.
 *                <p>
 *                Enabling the alarm starts polling the pir sensor. This
 *                function allows to check if any movement was detected by the
 *                pir sensor. The alarm can be reseted by calling resetAlarm.
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       1 for the pir has detected some movements, 0 otherwise
 *
 ******************************************************************************/
int32_t isAlarmSet(void) {

    return (isAlarmOn());
}

/*******************************************************************************
 *  function :    resetAlarm
 ******************************************************************************/
/** \brief        Resets the alarm.
 *                <p>
 *                If the pir detects any movement within the webhouse, the
 *                alarm is triggered. The alarm can be reseted by calling this
 *                function.
 *                <p>
 *                The webhouse must be initialized (initWebhouse) before this
 *                function can be called.
 *
 *  \type         global
 *
 *  \return       void
 *
 ******************************************************************************/
void resetAlarm(void) {

    resetAlarmPir();
}

/*******************************************************************************
 *  function :    initTV
 ******************************************************************************/
static BBBError initTV(void) {

    BBBError error = BBB_SUCCESS;

    error = exportGpio(GPIO_TV);
    error |= setGpioDirection(GPIO_TV, GPIO_DIR_OUT);
    error |= setGpioValue(GPIO_TV, GPIO_VALUE_LOW);

    return (error);
}

/*******************************************************************************
 *  function :    finalizeTV
 ******************************************************************************/
static BBBError finalizeTV(void) {

    BBBError error = BBB_SUCCESS;

    error = setGpioValue(GPIO_TV, GPIO_VALUE_LOW);
    error |= unexportGpio(GPIO_TV);

    return (error);
}

/*******************************************************************************
 *  function :    initLED
 ******************************************************************************/
static BBBError initLED(void) {

    BBBError error = BBB_SUCCESS;

    error = exportGpio(GPIO_LED);
    error |= setGpioDirection(GPIO_LED, GPIO_DIR_OUT);
    error |= setGpioValue(GPIO_LED, GPIO_VALUE_LOW);

    return (error);
}

/*******************************************************************************
 *  function :    finalizeLED
 ******************************************************************************/
static BBBError finalizeLED(void) {

    BBBError error = BBB_SUCCESS;

    error = setGpioValue(GPIO_LED, GPIO_VALUE_LOW);
    error |= unexportGpio(GPIO_LED);

    return (error);
}

/*******************************************************************************
 *  function :    initSLampe
 ******************************************************************************/
static BBBError initSLampe(void) {

    BBBError error = BBB_SUCCESS;

    error = setPwmState(PWM_P9_22, PWM_RUN);
    error |= setPwmPeriod(PWM_P9_22, PWM_PERIOD);
    error |= setPwmDuty(PWM_P9_22, PWM_PERIOD);

    return (error);
}

/*******************************************************************************
 *  function :    finalizeSLampe
 ******************************************************************************/
static BBBError finalizeSLampe(void) {

    return (setPwmState(PWM_P9_22, PWM_STOP));
}

/*******************************************************************************
 *  function :    initDLampe
 ******************************************************************************/
static BBBError initDLampe(void) {

    BBBError error = BBB_SUCCESS;

    error = setPwmState(PWM_P9_14, PWM_RUN);
    error |= setPwmPeriod(PWM_P9_14, PWM_PERIOD);
    error |= setPwmDuty(PWM_P9_14, PWM_PERIOD);

    return (error);
}

/*******************************************************************************
 *  function :    finalizeDLampe
 ******************************************************************************/
static BBBError finalizeDLampe(void) {

    return (setPwmState(PWM_P9_14, PWM_STOP));
}

/*******************************************************************************
 *  function :    initHeizung
 ******************************************************************************/
static BBBError initHeizung(void) {

    BBBError error = BBB_SUCCESS;

    error = setPwmState(PWM_P8_19, PWM_RUN);
    error |= setPwmPeriod(PWM_P8_19, PWM_PERIOD);
    error |= setPwmDuty(PWM_P8_19, PWM_PERIOD);

    return (error);
}

/*******************************************************************************
 *  function :    finalizeHeizung
 ******************************************************************************/
static BBBError finalizeHeizung(void) {

    return (setPwmState(PWM_P8_19, PWM_STOP));
}

/*******************************************************************************
 *  function :    calcDuration
 ******************************************************************************/
static uint64_t calcDuration(uint64_t u64StartTime, uint64_t u64StopTime) {

    uint64_t u64Duration = 0;

    if(u64StartTime > u64StopTime) {

        u64Duration = (UINT64_MAX - u64StartTime) + u64StopTime;

    } else {

        u64Duration = u64StopTime - u64StartTime;
    }

    return (u64Duration);
}

/*******************************************************************************
 *  function :    timespec2nsec
 ******************************************************************************/
static uint64_t timespec2nsec(struct timespec * time) {

    uint64_t u64TimeNsec = 0;

    u64TimeNsec += time->tv_nsec;
    u64TimeNsec += ((uint64_t) time->tv_sec) * ((uint64_t) 1000000000ULL);

    return (u64TimeNsec);
}
