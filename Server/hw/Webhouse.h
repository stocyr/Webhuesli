#ifndef WEBHOUSE_H_
#define WEBHOUSE_H_
/******************************************************************************/
/** \file       Webhouse.h
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
 ******************************************************************************/
/*
 *  function    initWebhouse
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
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <stdint.h>

#include "BBBTypes.h"

//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
extern BBBError initWebhouse(void);
extern BBBError finalizeWebhouse(void);

extern BBBError turnTVOn(void);
extern BBBError turnTVOff(void);
extern int32_t  getTVState(void);

extern BBBError turnLEDOn(void);
extern BBBError turnLEDOff(void);
extern int32_t  getLEDState(void);

extern BBBError dimSLampe(uint8_t u8Duty);
extern int32_t  getSLampeState(void);

extern BBBError dimDLampe(uint8_t u8Duty);
extern int32_t  getDLampeState(void);

extern BBBError dimHeizung(uint8_t u8Duty);
extern int32_t  getHeizungState(void);
extern int32_t  getTempIst(void);

extern BBBError enableAlarm(void);
extern BBBError disableAlarm(void);
extern int32_t  getAlarmState(void);
extern int32_t  isAlarmSet(void);
extern void     resetAlarm(void);

//----- Data -------------------------------------------------------------------


#endif /* WEBHOUSE_H_ */
