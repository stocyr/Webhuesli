/******************************************************************************/
/** \file       BBBSignal.c
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
 *  \date       August 2013
 *
 *  \remark     Last Modification
 *               \li wht4, August 2013, Created
 *
 ******************************************************************************/
/*
 *  functions  global:
 *              blockAllSignalForThread
 *              registerExitHandler
 *  functions  local:
 *              .
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "BBBSignal.h"
#include "Log.h"

//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------

//----- Data -------------------------------------------------------------------

//----- Implementation ---------------------------------------------------------



/*******************************************************************************
 *  function :    blockAllSignalForThread
 ******************************************************************************/
/** \brief        Calling this function in a thread will result in blocking
 *                all incoming signals for the calling thread.
 *                <p>
 *                All threads implemented in the system are calling this
 *                function. Thus you have to deal with signals in the main
 *                thread.
 *
 *  \type         global
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_BLOCK_SIGNAL if we could not block signals for the
 *                                 calling thread
 *                </pre>
 *
 ******************************************************************************/
BBBError blockAllSignalForThread(void) {

    BBBError  error = BBB_SUCCESS;
    sigset_t  sSigmask;

    sigfillset(&sSigmask);

    if (pthread_sigmask(SIG_BLOCK, &sSigmask, NULL) < 0) {
        ERRORPRINT("pthread_sigmask() failed");
        error = BBB_BLOCK_SIGNAL;
    }
    return (error);
}

/*******************************************************************************
 *  function :    registerExitHandler
 ******************************************************************************/
/** \brief        This function will register a callback function for the
 *                incoming signals SIGINT and SIGTERM.
 *                <p>
 *                All threads implemented in the generic part of the test system
 *                are blocking incoming signals. Thus you have to deal with
 *                signals in the main thread. This function will help you to
 *                do this.
 *
 *  \type         global
 *
 *  \param[in]    pfHandler    Callback function you like to register for
 *                             incoming signals (SIGINT and SIGTERM).
 *
 *  \return       <pre>
 *                BBB_SUCCESS        on success
 *                BBB_SIGNAL_HANDLER if we could not register callback function
 *                BBB_ERR_PARAM      if pfHandler is equal to <CODE>NULL</CODE>
 *                </pre>
 *
 ******************************************************************************/
BBBError registerExitHandler(void (*pfHandler)(int32_t s32Signal)) {

    BBBError         error = BBB_SUCCESS;
    sigset_t         sSigmask;
    struct sigaction sSigaction;

    if (pfHandler != NULL) {

        sigemptyset(&sSigmask);
        sigaddset(&sSigmask, SIGTERM);
        sigaddset(&sSigmask, SIGINT);

        sSigaction.sa_flags = 0;
        sSigaction.sa_mask = sSigmask;
        sSigaction.sa_handler = pfHandler;

        if (sigaction(SIGTERM, &sSigaction, NULL) < 0) {
            ERRORPRINT("sigaction() SIGTERM failed");
            error = BBB_SIGNAL_HANDLER;
        }
        if (sigaction(SIGINT, &sSigaction, NULL) < 0) {
            ERRORPRINT("sigaction() SIGINT failed");
            error = BBB_SIGNAL_HANDLER;
        }

    } else {
        ERRORPRINT("parameter error");
        error = BBB_ERR_PARAM;
    }

    return (error);
}
