/******************************************************************************/
/** \file       Pir.c
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
 *              detected, isAlarmOn() can be called. To start a new motion
 *              detection resetAlarmPir() must be called.
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
 *              initPir
 *              finalizePir
 *              startPollPirThread
 *              stopPollPirThread
 *              isPollThreadRunning
 *              isAlarmOn
 *              resetAlarmPir
 *  functions  local:
 *              pollPirThread
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <pthread.h>
#include <unistd.h>

#include "Pir.h"
#include "Gpio.h"
#include "Log.h"
#include "BBBSignal.h"

//----- Macros -----------------------------------------------------------------
#define GPIO_PIR         ( 30 )

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
static void * pollPirThread(void * pvData);

//----- Data -------------------------------------------------------------------
/** Thread wherein the gpio of the pir is polled with the system call poll(2) */
static pthread_t       idThread = 0;
/** Mutex to guard access to the mutual resource pirAlarm                     */
static pthread_mutex_t mutexAlaram = PTHREAD_MUTEX_INITIALIZER;
/** Set to TRUE if a motion was detected, FALSE otherwise                     */
static boolE           pirAlarm = FALSE;
/** Start polling the gpio                                                    */
static boolE           started = FALSE;

//----- Implementation ---------------------------------------------------------

/*******************************************************************************
 *  function :    initPir
 ******************************************************************************/
/** \brief        Initialize the pir module
 *                <p>
 *                Must be called before any other function of this module.
 *                Initializes the pir gpio pin as input with rising edge.
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
BBBError initPir(void) {

    BBBError error = BBB_SUCCESS;

    error = exportGpio(GPIO_PIR);
    error |= setGpioDirection(GPIO_PIR, GPIO_DIR_IN);
    error |= setGpioEdge(GPIO_PIR, GPIO_EDGE_RISING);

    if(pthread_create(&idThread,
                      NULL,
                      pollPirThread,
                      NULL) != 0) {

    	ERRORPRINT("can't create thread");
    	error = BBB_THREAD_CREATE;
    }

    return (error);
}

/*******************************************************************************
 *  function :    finalizePir
 ******************************************************************************/
BBBError finalizePir(void) {

    stopPollPir();
    pthread_mutex_destroy(&mutexAlaram);
    return (unexportGpio(GPIO_PIR));
}

/*******************************************************************************
 *  function :    startPollPirThread
 ******************************************************************************/
/** \brief        Start to poll the pir gpio for any movement within the webhouse
 *                <p>
 *                The thread will start to poll the gpio pin of the pir with the
 *                system call poll(2). initPir() must be called before this
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
BBBError startPollPir(void) {

    BBBError error = BBB_SUCCESS;

    resetAlarmPir();
    pthread_mutex_lock(&mutexAlaram);
	started = TRUE;
	pthread_mutex_unlock(&mutexAlaram);

    return (error);
}

/*******************************************************************************
 *  function :    stopPollPirThread
 ******************************************************************************/
/** \brief        Stop to poll the pir gpio for any movement within the webhouse
 *                <p>
 *                The thread will stop polling the gpio pin of the pir.initPir()
 *                must be called before this function can be called.
 *
 *  \type         global
 *
 *  \return       <pre>
 *                BBB_SUCCESS        on success
 *                BBB_THREAD_RUNNING thread is not running
 *                </pre>
 *
 ******************************************************************************/
BBBError stopPollPir(void) {

    BBBError error = BBB_SUCCESS;

    pthread_mutex_lock(&mutexAlaram);
	started = FALSE;
	pthread_mutex_unlock(&mutexAlaram);

    return (error);
}

/*******************************************************************************
 *  function :    isPollThreadRunning
 ******************************************************************************/
/** \brief        Returns TRUE if the thread wherein the gpio pin of the pir is
 *                polled.
 *                <p>
 *                initPir() must be called before this function can be called.
 *
 *  \type         global
 *
 *  \return       TRUE if the thread is running, FALSE otherwise
 *
 ******************************************************************************/
boolE isPollThreadRunning(void) {

    boolE running = FALSE;

    pthread_mutex_lock(&mutexAlaram);
    running = started;
	pthread_mutex_unlock(&mutexAlaram);

    return (running);
}

/*******************************************************************************
 *  function :    isAlarmOn
 ******************************************************************************/
/** \brief        Returns TRUE if any movement within the webhouse was detected.
 *                <p>
 *                If a movement was detected, the pir thread will set pirAlarm
 *                to TRUE. With the help of this function, the state of
 *                pirAlarm can be checked. To start a new motion detection, and
 *                thus setting pirAlarm to FALSE can be done with resetAlarmPir().
 *                initPir() must be called before this function can be called.
 *
 *  \type         global
 *
 *  \return       TRUE if movement was detected, FALSE otherwise
 *
 ******************************************************************************/
boolE isAlarmOn(void) {

    boolE isAlarm = FALSE;

    if(idThread != 0) {
        pthread_mutex_lock(&mutexAlaram);
        isAlarm = pirAlarm;
        pthread_mutex_unlock(&mutexAlaram);
    }

    return (isAlarm);
}

/*******************************************************************************
 *  function :    resetAlarmPir
 ******************************************************************************/
/** \brief        Reset the alarm to start a new motion detection.
 *                <p>
 *                If a movement was detected, the pir thread will set pirAlarm
 *                to TRUE. To start a new motion detection, resetAlarmPir()
 *                can be called. This will set pirAlarm to FALSE.
 *                initPir() must be called before this function can be called.
 *
 *  \type         global
 *
 *  \return       void
 *
 ******************************************************************************/
void resetAlarmPir(void) {

    if(idThread != 0) {
        pthread_mutex_lock(&mutexAlaram);
        pirAlarm = FALSE;
        pthread_mutex_unlock(&mutexAlaram);
    }
}

/*******************************************************************************
 *  function :    pollPirThread
 ******************************************************************************/
/** \brief        Polls the pir gpio and thus waits on any movement within the
 *                webhouse.
 *                <p>
 *                Thread will be started/stopped with a call to
 *                startPollPirThread respectively stopPollPirThread. Sets
 *                pirAlarm to TRUE if any movement was detected. To start a
 *                new motion detection, resetAlarmPir() can be called.
 *
 *  \type         static
 *
 *  \param[in]    pvData  not used
 *
 *  \return       not used
 *
 ******************************************************************************/
static void * pollPirThread(void * pvData) {

    BBBError error = BBB_SUCCESS;
    boolE    run = FALSE;

    /* Block all signals for this thread */
    blockAllSignalForThread();

    while(1) {

    	do {
    		usleep(100000);
    		pthread_mutex_lock(&mutexAlaram);
    		run = started;
    		pthread_mutex_unlock(&mutexAlaram);
    	} while (run == FALSE);

        /* Wait for infinite time for any movement event */
        error = pollGpio(GPIO_PIR, POLL_TIMEOUT_INF);

        if(error == BBB_SUCCESS) {
            pthread_mutex_lock(&mutexAlaram);
            if(pirAlarm != TRUE) {
                INFOPRINT("Alarm on");
                pirAlarm = TRUE;
            }
            pthread_mutex_unlock(&mutexAlaram);

            /* Sleep some time to avoid bouncing of the pir */
            sleep(2);
        }
    }

    pthread_exit(NULL);
}
