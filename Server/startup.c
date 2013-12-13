/******************************************************************************/
/** \file       startup.c
 *******************************************************************************
 *
 *  \brief      Main application for the beaglebone black webhouse
 *              <p>
 *              The application implements a socket server (ip: localhost,
 *              port:5000) and waits on a connection attempt of the client.
 *              <p>
 *              The client can control  different in- and outputs of the
 *              webhouse. The following objects are available:
 *              <ul>
 *              <li> dlampe: The ceiling lamp can be dimmed from 0 (dark) to
 *              100 (bright)
 *              <li> slampe: The floor lamp can be dimmed from 0 (dark) to
 *              100 (bright)
 *              <li> tv: The TV can be turned on/off
 *              <li> tempsoll: Set's the target temperature within the webhouse
 *              <li> tempist: The current temperature within the house
 *              <li> alarm: The alarm can be turned on/off
 *              <li> pir: The alarm was triggered by the pir
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
 *              main
 *  functions  local:
 *              shutdownHook
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "Webhouse.h"
#include "Log.h"
#include "BBBConfig.h"
#include "BBBSignal.h"
#include "Json.h"

//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
static void shutdownHook(int32_t sig);

//----- Data -------------------------------------------------------------------
static volatile boolE eShutdown = FALSE;

//----- Implementation ---------------------------------------------------------

/*******************************************************************************
 *  function :    main
 ******************************************************************************/
/** \brief        Starts the socket server (ip: localhost, port:5000) and waits
 *                on a connection attempt of the client.
 *
 *  \type         global
 *
 *  \return       EXIT_SUCCESS
 *
 ******************************************************************************/
int main(int argc, char **argv) {

    BBBError       error = BBB_SUCCESS;

    /* Initialize the webhouse */
    error = initWebhouse();

    INFOPRINT("Start of BBB webhouse");

    if((error == BBB_SUCCESS) &&
            (registerExitHandler(shutdownHook) == BBB_SUCCESS)) {

        while(eShutdown == FALSE) {

            /* sleep for 10ms */
            usleep(10000);
        }

    } else {
        ERRORPRINT("Failed to start BBB Webhouse");
    }

    /* Detach all resource */
    finalizeWebhouse();
    INFOPRINT("Stop of BBB webhouse");
    return EXIT_SUCCESS;
}

/*******************************************************************************
 *  function :    shutdownHook
 ******************************************************************************/
/** \brief        Handle the registered signals (SIGTERM, SIGINT)
 *
 *  \type         static
 *
 *  \param[in]    sig    incoming signal
 *
 *  \return       void
 *
 ******************************************************************************/
static void shutdownHook(int32_t sig) {

    INFOPRINT("Ctrl-C pressed....shutdown hook in main");
    eShutdown = TRUE;
}

