#ifndef PIR_H_
#define PIR_H_
/******************************************************************************/
/** \file       Pir.h
 *******************************************************************************
 *
 *  \brief      With the help of the pir, movements within the webhouse can be
 *              detected.
 *              <p>
 *              The pir is an input pin (gpio30, active high) of the beaglebone
 *              black webhouse. Before you can use other functions of this
 *              module, initPir() must be called. Afterwards you can start/stop
 *              detecting motions with startPollPirThread() or
 *              stopPollPirThread(). You can check if the poll thread is running
 *              by calling isPollThreadRunning(). To see if a motion was
 *              detected, isAlarmOn() can be called. To start a neew motion
 *              detection resetAlarmPir() must be called.
 *
 *  \author     wht4
 *
 ******************************************************************************/
/*
 *  function    initPir
 *              finalizePir
 *              startPollPirThread
 *              stopPollPirThread
 *              isPollThreadRunning
 *              isAlarmOn
 *              resetAlarmPir
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <stdint.h>

#include "BBBTypes.h"

//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
extern BBBError initPir(void);

extern BBBError finalizePir(void);

extern BBBError startPollPir(void);

extern BBBError stopPollPir(void);

extern boolE    isPollThreadRunning(void);

extern boolE    isAlarmOn(void);

extern void     resetAlarmPir(void);

//----- Data -------------------------------------------------------------------

#endif /* PIR_H_ */
