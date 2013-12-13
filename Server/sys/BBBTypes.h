#ifndef BBBTYPES_H_
#define BBBTYPES_H_
/******************************************************************************/
/** \file       BBBTypes.h
 *******************************************************************************
 *
 *  \brief      All data types used within the beaglebone black webhouse
 *              system.
 *              <p>
 *              Most functions will return an error of type
 *              <CODE>BBBError</CODE>. All possible errors are explained
 *              in this file.
 *
 *  \author     wht4
 *
 ******************************************************************************/
/*
 *  function    .
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------

//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------

/** Definition of boolean type */
typedef enum _boolE {
    FALSE = 0,     ///< boolean value for false
    TRUE  = 1      ///< boolean value for true
} boolE;

/**
 * Error type used as a return value by most functions within the webhouse
 */
typedef enum _BBBError {

    BBB_SUCCESS           = 0,  ///< No error occurred
    BBB_ERR_UNKNOWN       = 1,  ///< A non further specified error occurred
    BBB_ERR_PARAM         = 2,  ///< A passed parameter has a wrong value, size or type

    BBB_FILE_OPEN         = 10, ///< Could not open file
    BBB_FILE_CLOSE        = 11, ///< Could not close file
    BBB_FILE_IOCTL        = 12, ///< Error in device control
    BBB_FILE_READ         = 13, ///< Error on read file

    BBB_GPIO_POLL         = 20, ///< Error on polling a gpio

    BBB_SOCKET_SOCKET     = 30, ///< Socket socket() failed
    BBB_SOCKET_OPT        = 31, ///< sock_opt() failed
    BBB_SOCKET_LISTEN     = 32, ///< Socket listen() failed
    BBB_SOCKET_BIND       = 33, ///< Socket bind() failed
    BBB_SOCKET_ACCEPT     = 34, ///< Socket accept() failed
    BBB_SOCKET_SEND       = 35, ///< Socket send() failed
    BBB_SOCKET_CLOSED     = 36, ///< Socket was closed by client
    BBB_SOCKET_NO_MSG     = 37, ///< Socket no message available on recv()

    BBB_JSON_PACK         = 40, ///< We could not pack a new json message
    BBB_JSON_CREATE       = 41, ///< We could not create json message

    BBB_RINGB_SIZE        = 50, ///< The passed buffer is to small to hold full message content
    BBB_RINGB_FULL        = 51, ///< You try to write into a ringbuffer which is full
    BBB_RINGB_EMPTY       = 52, ///< You try to read from a ringbuffer which is empty

    BBB_BLOCK_SIGNAL      = 60, ///< We could not block signals for the calling thread
    BBB_SIGNAL_HANDLER    = 61, ///< Could not register callback function

    BBB_THREAD_RUNNING    = 70, ///< The thread is allready running
    BBB_THREAD_CREATE     = 71, ///< The thread could not be created

    BBB_CMD_INVALID       = 80, ///< An invalid cmd object was received
    BBB_CMD_NO_CMD        = 81, ///< No cmd object is available
    BBB_CMD_COMPOSE_MSG   = 82  ///< The response could not be composed

} BBBError;

//----- Function prototypes ----------------------------------------------------

//----- Data -------------------------------------------------------------------

#endif /* BBBTYPES_H_ */
