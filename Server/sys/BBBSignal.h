#ifndef BBBSIGNAL_H_
#define BBBSIGNAL_H_
/******************************************************************************/
/** \file       BBBSignal.h
 *******************************************************************************
 *
 *  \brief      This is a helper module to handle the signal API.
 *              <p>
 *              The beaglebone black webhouse must handle the most important
 *              incoming signals. Thus all threads except the main thread are
 *              blocking incoming signals. The main thread can register an exit
 *              handler for the signals SIGTERM and SIGINT and terminate the
 *              system in a controlled way.
 *
 *  \author     wht4
 *
 ******************************************************************************/
/*
 *  function    blockAllSignalForThread
 *              registerExitHandler
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <stdint.h>

#include "BBBTypes.h"

//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
extern BBBError blockAllSignalForThread(void);

extern BBBError registerExitHandler(void (*pfHandler)(int32_t s32Signal));

//----- Data -------------------------------------------------------------------


#endif /* BBBSIGNAL_H_ */
